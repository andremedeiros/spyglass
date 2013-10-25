require 'spec_helper'

describe Spyglass::Image do
  let(:lena) { Spyglass::Image.load(fixture_path('lena.jpg')) }

  describe '.load' do
    it 'should require an argument' do
      expect { Spyglass::Image.load }.to raise_error ArgumentError
    end

    it 'should return an instance of OpenCV::Image when passing a path' do
      expect( Spyglass::Image.load(fixture_path('lena.jpg')) ).to be_a described_class
    end
  end

  describe '.zeros' do
    it 'should require a size' do
      expect { Spyglass::Image.zeros }.to raise_error ArgumentError

      expect( Spyglass::Image.zeros(Spyglass::Size.new(20, 20)) ).to be_a Spyglass::Image
    end

    it 'should create an image filled with zeros' do
      img = Spyglass::Image.zeros Spyglass::Size.new(50, 50)
      mean = img.mean

      expect( mean.zeros? ).to be_true
    end
  end

  describe 'accessors' do
    describe '#rows' do
      it 'should return the right number of rows for an image' do
        expect( lena.rows ).to eq(512)
      end
    end

    describe '#cols' do
      it 'should return the right number of columns for an image' do
        expect( lena.cols ).to eq(512)
      end
    end

    describe '#size' do
      it 'should be an instance of Spyglass::Size' do
        expect( lena.size ).to be_a Spyglass::Size
      end

      it 'should return the correct values' do
        expect( lena.size.width ).to eq( lena.cols )
        expect( lena.size.height ).to eq( lena.rows )
      end
    end
  end

  describe '#erode' do
    it 'should erode the image' do
      eroded = lena.erode

      expect( eroded.cols ).to eq( lena.cols )
      expect( eroded.rows ).to eq( lena.rows )
    end
  end

  describe '#crop' do
    it 'should crop the image to the correct proportions' do
      rect = Spyglass::Rect.new(0, 0, 256, 256)
      cropped = lena.crop(rect)

      expect( cropped.cols ).to eq(256)
      expect( cropped.rows ).to eq(256)
    end
  end

  describe '#crop!' do
    it 'should crop the image in place to the correct proportions' do
      rect = Spyglass::Rect.new(0, 0, 256, 256)
      lena.crop!(rect)

      expect( lena.cols ).to eq(256)
      expect( lena.rows ).to eq(256)
    end
  end

  describe '#mean' do
    it 'should return a Spyglass::Color' do
      expect( lena.mean ).to be_a Spyglass::Color
    end
  end

  describe '#write' do
    it 'should write the image onto disk' do
      Dir.mktmpdir do |dir|
        path = File.join(dir, 'lena.jpg')

        res = lena.write(path)
        expect( File.exists?(path) ).to be_true
        expect( res ).to be_true
      end
    end
  end

  describe '#copy!' do
    it 'should copy the image inplace' do
      im = Spyglass::Image.new

      expect( im.rows ).to eq(0)
      expect( im.cols ).to eq(0)

      im.copy!(lena)

      expect( im.rows ).to eq(512)
      expect( im.cols ).to eq(512)
    end
  end

  describe '#resize' do
    it 'should resize the image' do
      new_img = lena.resize(Spyglass::Size.new(1024, 1024))

      expect( new_img.rows ).to eq(1024)
      expect( new_img.cols ).to eq(1024)
    end
  end

  describe '#resize!' do
    it 'should resize the image in place' do
      lena.resize!(Spyglass::Size.new(1024, 1024))

      expect( lena.rows ).to eq(1024)
      expect( lena.cols ).to eq(1024)
    end
  end

  describe '#color_at' do
    let(:colors) { Spyglass::Image.load(fixture_path('rgb.png')) }

    it 'should return the correct values' do
      red   = colors.color_at(Spyglass::Point.new(155, 55))
      green = colors.color_at(Spyglass::Point.new(70, 215))
      blue  = colors.color_at(Spyglass::Point.new(245, 215))
      white = colors.color_at(Spyglass::Point.new(155, 155))

      expect( red.to_a   ).to eq( [0, 0, 255, 0] )
      expect( green.to_a ).to eq( [0, 255, 0, 0] )
      expect( blue.to_a  ).to eq( [255, 0, 0, 0] )
      expect( white.to_a ).to eq( [255, 255, 255, 0] )
    end
  end
end
