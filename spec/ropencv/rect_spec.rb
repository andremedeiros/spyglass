require 'spec_helper'

describe OpenCV::Rect do
  let(:rect) { OpenCV::Rect.new(10, 20, 100, 200) }

  describe '.new' do
    it 'should require four arguments' do
      expect { OpenCV::Rect.new }.to raise_error ArgumentError
      expect( rect ).to be_a OpenCV::Rect
    end
  end

  describe 'accessors' do
    describe '#x' do
      it 'should return the correct X coordinate' do
        expect( rect.x ).to eq(10)
      end
    end

    describe '#y' do
      it 'should return the correct Y coordinate' do
        expect( rect.y ).to eq(20)
      end
    end

    describe '#width' do
      it 'should return the correct value' do
        expect( rect.width ).to eq(100)
      end
    end

    describe '#height' do
      it 'should return the correct value' do
        expect( rect.height ).to eq(200)
      end
    end

    describe '#area' do
      it 'should return `width * height`' do
        expect( rect.area ).to eq(20_000)
      end
    end

    describe '#size' do
      it 'should be an instance of OpenCV::Size' do
        expect( rect.size ).to be_a OpenCV::Size
      end

      it 'should return the correct values' do
        expect( rect.size.width ).to eq( rect.width )
        expect( rect.size.height ).to eq( rect.height )
      end
    end

    describe '#point' do
      it 'should be an instance of OpenCV::Point' do
        expect( rect.point ).to be_a OpenCV::Point
      end

      it 'should return the correct values' do
        expect( rect.point.x ).to eq( rect.x )
        expect( rect.point.y ).to eq( rect.y )
      end
    end
  end

  describe 'setters' do
    describe '#height=' do
      it 'should set the value of the height' do
        rect.height = 150

        expect( rect.height ).to eq(150)
        expect( rect.area ).to eq(15_000)
      end
    end

    describe '#width=' do
      it 'should set the value of the width' do
        rect.width = 300

        expect( rect.width ).to eq(300)
        expect( rect.area ).to eq(60_000)
      end
    end

    describe '#x=' do
      it 'should set the value of the X coordinate' do
        rect.x = 20

        expect( rect.x ).to eq(20)
      end
    end

    describe '#y=' do
      it 'should set the value of the Y coordinate' do
        rect.y = 40

        expect( rect.y ).to eq(40)
      end
    end
  end

end
